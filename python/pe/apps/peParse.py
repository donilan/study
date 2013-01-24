import struct
from collections import namedtuple

ImageDOSHeaderStr = """
2s magic
H cblp
H cp
H crlc
H cparhdr
H minalloc
H maxalloc
H ss
H sp
H csum
H ip
H cs
H lfarlc
H ovno
8s res
H oemid
H oeminfo
20s res2
L lfanew
"""

PESignatureStr = """
4s signature
"""

FileHeaderStr = """
2s machine
H numberOfSections
H timeDateStamp
L pointerToSymbolTable
L numberOfSymbols
H sizeOfOptionalHeader
H characteristics
"""

OptionalHeaderStr = """
2s magic
s majorLinkerVersion
s minorLinkerVersion
L sizeOfCode
L sizeOfInitializedData
L sizeOfUninitializedData
L addressOfEntryPoint
L baseOfCode
L baseOfData
L imageBase
L sectionAlignment
L fileAlignment
H majorOperatingSystemVersion
H minorOperatingSystemVersion
H majorImageVersion
H minorImageVersion
H majorSubsytemVersion
H minorSubsystemVersion
L win32VersionValue
L sizeOfImage
L sizeOfHeaders
L checkSum
H subsystem
H DLLCharacteristics
L sizeOfStackReserve
L sizeOfStackCommit
L sizeOfHeapReserve
L sizeOfHeapCommit
L loaderFlags
L numberOfRvaAndSizes
"""

ImageDataDirectoryStr = """
L virtualAddress
L size
"""
ImageDataDirectoryNames = [
'IMAGE_DIRECTORY_ENTRY_EXPORT',
'<span class="important">IMAGE_DIRECTORY_ENTRY_IMPORT</span>',
'IMAGE_DIRECTORY_ENTRY_RESOURCE',
'IMAGE_DIRECTORY_ENTRY_EXCEPTION',
'IMAGE_DIRECTORY_ENTRY_SECURITY',
'IMAGE_DIRECTORY_ENTRY_BASERELOC',
'IMAGE_DIRECTORY_ENTRY_DEBUG',
'IMAGE_DIRECTORY_ENTRY_COPYRIGHT',
'IMAGE_DIRECTORY_ENTRY_GLOBALPTR',
'IMAGE_DIRECTORY_ENTRY_TLS',
'IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG',
'IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT',
'IMAGE_DIRECTORY_ENTRY_IAT',
'IMAGE_DIRECTORY',
'IMAGE_DIRECTORY',
'IMAGE_DIRECTORY',
'IMAGE_DIRECTORY',
'IMAGE_DIRECTORY',
'IMAGE_DIRECTORY',
'IMAGE_DIRECTORY',
'IMAGE_DIRECTORY',
]

SectionStr = """
8s name
L virtualSize
L RVA
L dataAlignSize
L dataOffset
L relocationsOffset
L lineNumbersOffset
H numRelocations
H numLineNumbers
L flags
"""

def read(f):
    data = f.read()
    fileSize = len(data)
    dosH = toStruct(ImageDOSHeaderStr, data)
    signature = toStruct(PESignatureStr, data, dosH.lfanew)
    currentPos = dosH.lfanew+signature.structLength
    fileHeader = toStruct(FileHeaderStr, data, currentPos)
    currentPos += fileHeader.structLength
    optionalHeader = toStruct(OptionalHeaderStr, data, currentPos)
    currentPos += optionalHeader.structLength
    imageDataDirectorys = []
    for i in range(optionalHeader.numberOfRvaAndSizes):
        tmp = toStruct(ImageDataDirectoryStr, data, currentPos, {'imageDataDirectoryName': ImageDataDirectoryNames[i]})
        currentPos += tmp.structLength
        imageDataDirectorys.append(tmp)

    sections = []
    for i in range(fileHeader.numberOfSections):
        tmp = toStruct(SectionStr, data, currentPos)
        currentPos += tmp.structLength
        sections.append(tmp)
    
    result =  {
        'fileSize': fileSize,
        'dosHeader': dosH,
        'peSignature': signature,
        'fileHeader': fileHeader,
        'optionalHeader': optionalHeader,
        'imageDateDirectorys': imageDataDirectorys,
        'sections': sections}

    return result

def toStruct(_str, data, offset=None, extends={}):
    """
    Translate data to struct.
    """

    tmp = zip(*[line.split() for line in _str.strip().splitlines()])
    fields = list(tmp[1])
    # join pack str like '4sHHHLLL'
    packStr = ''.join(tmp[0])
    
    # calc struct length
    structLength = struct.calcsize(packStr)
    # unpack data
    unpackData = data[offset or 0 : (offset or 0) + structLength]
    args = struct.unpack(packStr, unpackData)

    # add length to struct
    fields.append('structLength')
    args += (structLength, )

    # add extends fields
    fields += extends.keys()
    args += tuple(extends.values())
    print fields
    print args

    # fieldStr like 'field1 field2 field3 field4'
    fieldStr = ' '.join(fields)

    # Make struct
    Struct = namedtuple('Struct', fieldStr)
    return Struct(*args)


