from collections import namedtuple
import struct

IMAGE_DOS_HEADER_FIELD = 'magic cblp cp crlc cparhdr minalloc maxalloc ss sp csum ip cs lfarlc ovno res oemid oeminfo res2 lfanew'.split()
ImageDOSHeader = namedtuple('ImageDOSHeader', IMAGE_DOS_HEADER_FIELD)
DOS_HEADER_FMT = '2sHHHHHHHHHHHHH8sHH20sL'

IMAGE_FILE_HEADER_FIELD = 'machine numberOfSections timeDateStamp pointerToSymbolTable numberOfSymbols sizeOfOptionalHeader characteeristice'.split()
ImageFileHeader = namedtuple('ImageFileHeader', IMAGE_FILE_HEADER_FIELD)
FILE_HEADER_FMT = '2sHLLLH2s'


_TYPE_MAP = {ImageDOSHeader: DOS_HEADER_FMT,
             ImageFileHeader: FILE_HEADER_FMT}


def parse(data, typ, offset=None):
    fmt = _TYPE_MAP[typ]
    args = struct.unpack(fmt, data[offset or 0: 
                                   (offset or 0 ) + struct.calcsize(fmt)])
    return typ(*args)

