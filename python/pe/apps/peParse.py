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

def read(f):
    data = f.read()
    dosH = toStruct(ImageDOSHeaderStr, data)
    signature = toStruct(PESignatureStr, data, dosH.lfanew)
    currentPos = dosH.lfanew+signature.structLength
    fileHeader = toStruct(FileHeaderStr, data, currentPos)
    currentPos += fileHeader.structLength
    optionalHeader = toStruct(OptionalHeaderStr, data, currentPos)
    currentPos += optionalHeader.structLength
    imageDataDirectory = []
    for i in range(16):
        tmp = toStruct(ImageDataDirectoryStr, data, currentPos)
        currentPos += tmp.structLength
        imageDataDirectory.append(tmp)
    print imageDataDirectory


def toStruct(_str, data, offset=None):
    tmp = zip(*[line.split() for line in _str.strip().splitlines()])
    packStr = ''.join(tmp[0])
    fieldStr = ' '.join(tmp[1]) + ' structLength'
    Struct = namedtuple('Struct', fieldStr)
    structLength = struct.calcsize(packStr)
    args = struct.unpack(packStr, data[offset or 0 : (offset or 0) + structLength]) + (structLength, )

    result = Struct(*args)
    return result


