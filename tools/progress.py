#!/usr/bin/env python3
import argparse, csv, git, json, os, re, sys

parser = argparse.ArgumentParser()

parser = argparse.ArgumentParser(description="Computes current progress throughout the whole project.")
parser.add_argument("format", nargs="?", default="text", choices=["text", "csv", "shield-json"])
parser.add_argument("-m", "--matching", dest='matching', action='store_true',
                    help="Output matching progress instead of decompilation progress")
args = parser.parse_args()

NON_MATCHING_PATTERN = r'#ifdef\s+NON_MATCHING.*?#pragma\s+GLOBAL_ASM\s*\(\s*"(.*?)"\s*\).*?#endif'
NOT_ATTEMPTED_PATTERN = r'#pragma\s+GLOBAL_ASM\s*\(\s*"(.*?)"\s*\)'

# This is the format ZAPD uses to autogenerate variable names
# It should not be used for properly documented variables
AUTOGENERATED_ASSET_NAME = re.compile(r".+0[0-9A-Fa-f]{5}")

ASM_JMP_LABEL = re.compile(r"^(?P<name>L[0-9A-F]{8})$")

# TODO: consider making this a parameter of this script
GAME_VERSION = "mm.us.rev1"

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

def GetFunctionsByPattern(pattern, files):
    functions = []

    for file in files:
        with open(file) as f:
            functions += re.findall(pattern, f.read(), re.DOTALL)

    return functions

def ReadAllLines(fileName):
    line_list = list()
    with open(fileName) as f:
        line_list = f.readlines()

    return line_list

def GetFiles(path, ext):
    files = []

    for r, d, f in os.walk(path):
        for file in f:
            if file.endswith(ext):
                files.append(os.path.join(r, file))

    return files

def GetCsvFilelist(version, filelist):
    path = os.path.join("tools", "filelists", version, filelist)
    with open(path, newline='') as f:
        return list(csv.reader(f, delimiter=','))

def GetRemovableSize(functionSizes, functions_to_count):
    size = 0

    for func in functions_to_count:
        if func in functionSizes:
            size += functionSizes[func]

    return size

def CalculateMapSizes(mapFileList):
    for mapFile in mapFileList:
        accumulatedSize = 0

        symbolCount = len(mapFile["symbols"])
        if symbolCount == 0:
            continue

        # Calculate size of each symbol
        for index in range(symbolCount - 1):
            symbol = mapFile["symbols"][index]
            nextSymbol = mapFile["symbols"][index+1]

            size = nextSymbol["vram"] - symbol["vram"]
            accumulatedSize += size

            mapFile["symbols"][index]["size"] = size

        # Calculate size of last symbol of the file
        symbol = mapFile["symbols"][-1]
        size = mapFile["size"] - accumulatedSize
        mapFile["symbols"][-1]["size"] = size
    return mapFileList

def GetFunctionSizes(mapFileList):
    functionSizes = dict()

    for mapFile in mapFileList:
        if mapFile["section"] != ".text":
            continue

        for symbol in mapFile["symbols"]:
            symbolName = symbol["name"]
            functionSizes[symbolName] = symbol["size"]

    return functionSizes

def CalculateNonNamedAssets(mapFileList, assetsTracker):
    for mapFile in mapFileList:
        if mapFile["section"] != ".data":
            continue
        if not mapFile["name"].startswith("build/assets/"):
            continue

        assetCat = mapFile["name"].split("/")[2]

        for symbol in mapFile["symbols"]:
            symbolName = symbol["name"]
            if AUTOGENERATED_ASSET_NAME.search(symbolName) is not None:
                if assetCat in assetsTracker:
                    assetsTracker[assetCat]["removableSize"] += symbol["size"]
    return assetsTracker


map_file = ReadAllLines('build/mm.map')

# Get list of Non-Matchings
all_files = GetFiles("src", ".c")
non_matching_functions = GetFunctionsByPattern(NON_MATCHING_PATTERN, all_files)

# Get list of functions not attempted.
not_attempted_functions = GetFunctionsByPattern(NOT_ATTEMPTED_PATTERN, all_files)
not_attempted_functions = list(set(not_attempted_functions).difference(non_matching_functions))

# If we are looking for a count that includes non-matchings, then we want to set non matching functions list to empty.
# We want to do this after not attempted functions list generation so we can remove all non matchings.
if not args.matching:
    non_matching_functions = []

# The order of this list should not change to prevent breaking the graph of the website
# New stuff shall be appended at the end of the list
assetsCategories = [
    "archives",
    "audio",
    "interface",
    "misc",
    "objects",
    "scenes",
    "text",
    # "deleted",
    # "segments",
]
assetsTracker = dict()

# Manual fixer for files that would be counted in wrong categories
# "filename": "correctSection"
fileSectionFixer = {
    "osFlash": "code" # Currently in `src/libultra` (would be counted as boot)
}

for assetCat in assetsCategories:
    assetsTracker[assetCat] = dict()
    # Get asset files
    assetsTracker[assetCat]["files"] = GetCsvFilelist(GAME_VERSION, f"{assetCat}.csv")
    assetsTracker[assetCat]["currentSize"] = 0
    assetsTracker[assetCat]["removableSize"] = 0
    assetsTracker[assetCat]["totalSize"] = 0
    assetsTracker[assetCat]["percent"] = 0


# Initialize all the code values
srcCategories = [
    "boot",
    "libultra",
    "code",
    "overlays",
]

srcCategoriesFixer = {
    "boot_O2": "boot",
    "boot_O2_g3": "boot",
}

srcTracker = dict()
asmTracker = dict()

for srcCat in srcCategories:
    srcTracker[srcCat] = dict()
    srcTracker[srcCat]["currentSize"] = 0
    srcTracker[srcCat]["totalSize"] = 0
    srcTracker[srcCat]["percent"] = 0

    asmTracker[srcCat] = dict()
    asmTracker[srcCat]["currentSize"] = 0
    asmTracker[srcCat]["totalSize"] = 0
    asmTracker[srcCat]["percent"] = 0

mapFileList = []

for line in map_file:
    line_split =  list(filter(None, line.split(" ")))

    if (len(line_split) == 4 and line_split[0].startswith(".")):
        section = line_split[0]
        obj_vram = int(line_split[1], 16)
        file_size = int(line_split[2], 16)
        obj_file = line_split[3].strip()
        objFileSplit = obj_file.split("/")

        fileData = {"name": obj_file, "vram": obj_vram, "size": file_size, "section": section, "symbols": []}
        mapFileList.append(fileData)

        if (section == ".text"):
            objFileName = objFileSplit[-1].split(".o")[0]
            srcCat = obj_file.split("/")[2]
            if srcCat in srcCategoriesFixer:
                srcCat = srcCategoriesFixer[srcCat]

            if objFileName in fileSectionFixer:
                correctSection = fileSectionFixer[objFileName]
                if correctSection in srcTracker:
                    srcTracker[correctSection]["totalSize"] += file_size
            elif obj_file.startswith("build/src"):
                if srcCat in srcTracker:
                    srcTracker[srcCat]["totalSize"] += file_size
            elif (obj_file.startswith("build/asm")):
                if srcCat in asmTracker:
                    asmTracker[srcCat]["totalSize"] += file_size

        if section == ".data":
            if obj_file.startswith("build/assets/"):
                assetCat = obj_file.split("/")[2]
                if assetCat in assetsTracker:
                    assetsTracker[assetCat]["currentSize"] += file_size
                else:
                    eprint(f"Found file '{obj_file}' in unknown asset category '{assetCat}'")
                    eprint("I'll ignore this for now, but please fix it!")

    elif len(line_split) == 2 and line_split[0].startswith("0x00000000"):
        varVramStr, varName = line_split
        varVram = int(varVramStr, 16)
        varName = varName.strip()
        if varName == "0x0":
            continue
        if ASM_JMP_LABEL.search(varName) is not None:
            # Filter out jump table's labels
            continue
        symbolData = {"name": varName, "vram": varVram, "size": 0}
        mapFileList[-1]["symbols"].append(symbolData)

mapFileList = CalculateMapSizes(mapFileList)
functionSizes = GetFunctionSizes(mapFileList)

assetsTracker = CalculateNonNamedAssets(mapFileList, assetsTracker)


# Add libultra to boot.
srcTracker["boot"]["totalSize"] += srcTracker["libultra"]["totalSize"]
asmTracker["boot"]["totalSize"] += asmTracker["libultra"]["totalSize"]
del srcTracker["libultra"]
del asmTracker["libultra"]

# Calculate Non-Matching
non_matching_functions_ovl = list(map(lambda x: x.split("/")[-1].split(".")[0], filter(lambda x: "/overlays/" in x, non_matching_functions)))
non_matching_functions_code = list(map(lambda x: x.split("/")[-1].split(".")[0], filter(lambda x: "/code/" in x, non_matching_functions)))
non_matching_functions_boot = list(map(lambda x: x.split("/")[-1].split(".")[0], filter(lambda x: "/boot/" in x, non_matching_functions)))

non_matching_asm_ovl = GetRemovableSize(functionSizes, non_matching_functions_ovl)
non_matching_asm_code = GetRemovableSize(functionSizes, non_matching_functions_code)
non_matching_asm_boot = GetRemovableSize(functionSizes, non_matching_functions_boot)

# Calculate Not Attempted
not_attempted_functions_ovl = list(map(lambda x: x.split("/")[-1].split(".")[0], filter(lambda x: "/overlays/" in x, not_attempted_functions)))
not_attempted_functions_code = list(map(lambda x: x.split("/")[-1].split(".")[0], filter(lambda x: "/code/" in x, not_attempted_functions)))
not_attempted_functions_boot = list(map(lambda x: x.split("/")[-1].split(".")[0], filter(lambda x: "/boot/" in x, not_attempted_functions)))

not_attempted_asm_ovl = GetRemovableSize(functionSizes, not_attempted_functions_ovl)
not_attempted_asm_code = GetRemovableSize(functionSizes, not_attempted_functions_code)
not_attempted_asm_boot = GetRemovableSize(functionSizes, not_attempted_functions_boot)

# All the non matching asm is the sum of non-matching code
non_matching_asm = non_matching_asm_ovl + non_matching_asm_code + non_matching_asm_boot

# All the not attempted asm is the sum of not attemped code
not_attempted_asm = not_attempted_asm_ovl + not_attempted_asm_code + not_attempted_asm_boot

# Calculate total decompiled for each bucket by taking out the non-matching and not attempted in ovl/code/boot buckets.
srcTracker["code"]["currentSize"] = srcTracker["code"]["totalSize"] - (non_matching_asm_code + not_attempted_asm_code)
srcTracker["boot"]["currentSize"] = srcTracker["boot"]["totalSize"] - (non_matching_asm_boot + not_attempted_asm_boot)
srcTracker["overlays"]["currentSize"] = srcTracker["overlays"]["totalSize"] - (non_matching_asm_ovl + not_attempted_asm_ovl)

# Total code bucket sizes
handwritten = 0
for srcCat in asmTracker:
    handwritten += asmTracker[srcCat]["totalSize"]

# Calculate the total amount of decompilable code
total = 0
for srcCat in asmTracker:
    total += srcTracker[srcCat]["totalSize"]

# Calculate size of all assets
for assetCat in assetsTracker:
    for index, f in assetsTracker[assetCat]["files"]:
        assetsTracker[assetCat]["totalSize"] += os.stat(os.path.join("baserom", f)).st_size

if args.matching:
    for assetCat in assetsTracker:
        assetsTracker[assetCat]["currentSize"] -= assetsTracker[assetCat]["removableSize"]

# Calculate asm and src totals
src = 0
for srcCat in srcTracker:
    src += srcTracker[srcCat]["currentSize"]
asm = 0
for srcCat in asmTracker:
    asm += asmTracker[srcCat]["totalSize"]
asm += non_matching_asm + not_attempted_asm

# Calculate assets totals
assets = sum(x["currentSize"] for x in assetsTracker.values())
assets_total = sum(x["totalSize"] for x in assetsTracker.values())

# Convert vaules to percentages
src_percent = 100 * src / total
asm_percent = 100 * asm / total
for srcCat in ["boot", "code", "overlays"]:
    srcTracker[srcCat]["percent"] = 100 * srcTracker[srcCat]["currentSize"] / srcTracker[srcCat]["totalSize"]

assets_percent = 100 * assets / assets_total

for assetCat in assetsTracker:
    assetsTracker[assetCat]["percent"] = 100 * assetsTracker[assetCat]["currentSize"] / assetsTracker[assetCat]["totalSize"]

# convert bytes to masks and rupees
num_masks = 24
max_rupees = 500
bytes_per_mask = total / num_masks
bytes_per_rupee = bytes_per_mask / max_rupees
masks = int(src / bytes_per_mask)
rupees = int((src % bytes_per_mask) / bytes_per_rupee)

if args.format == 'csv':
    version = 2
    git_object = git.Repo().head.object
    timestamp = str(git_object.committed_date)
    git_hash = git_object.hexsha
    csv_list = [
        version, timestamp, git_hash, src, total,
    ]
    for srcCat in ["boot", "code", "overlays"]:
        csv_list += [srcTracker[srcCat]["currentSize"], srcTracker[srcCat]["totalSize"]]

    csv_list += [
        asm, len(non_matching_functions),
    ]
    csv_list += [
        assets, assets_total,
    ]
    for assetCat in assetsCategories:
        csv_list += [assetsTracker[assetCat]["currentSize"], assetsTracker[assetCat]["totalSize"]]

    print(",".join(map(str, csv_list)))
elif args.format == 'shield-json':
    # https://shields.io/endpoint
    print(json.dumps({
        "schemaVersion": 1,
        "label": "progress",
        "message": f"{src_percent:.3g}%",
        "color": 'yellow',
    }))
elif args.format == 'text':
    adjective = "decompiled" if not args.matching else "matched"
    assetsAdjective = "debinarized" if not args.matching else "identified"

    print("src:    {:>9} / {:>8} total bytes {:<13} {:>9.4f}%".format(src, total, adjective, round(src_percent, 4)))

    for srcCat in ["boot", "code", "overlays"]:
        src = srcTracker[srcCat]
        print("    {:<10}  {:>9} / {:>8} bytes {:<13} {:>9.4f}%".format(f"{srcCat}:", src["currentSize"], src["totalSize"], adjective, round(src["percent"], 4)))
    print()

    print("assets: {:>9} / {:>8} total bytes {:<13} {:>9.4f}%".format(assets, assets_total, assetsAdjective, round(assets_percent, 4)))
    for assetCat in assetsTracker:
        data = assetsTracker[assetCat]
        print("    {:<10}  {:>9} / {:>8} bytes {:<13} {:>9.4f}%".format(f"{assetCat}:", data["currentSize"], data["totalSize"], assetsAdjective, round(data["percent"], 4)))

    print()
    print("------------------------------------\n")

    if (rupees > 0):
        print('You have {}/{} masks and {}/{} rupee(s).\n'.format(masks, num_masks, rupees, max_rupees))
    else:
        print('You have {}/{} masks.\n'.format(masks, num_masks))
else:
    print("Unknown format argument: " + args.format)
