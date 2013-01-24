from django import template


register = template.Library()

_SECTION_FLAGS = {
    0x00000008: "Section should not be padded to next boundary.",
    0x00000020: "<span class='important' >Section contains code.</span>",
    0x00000040: "Section contains initialised data (which will become initialised with real values before the file is launched).",
    0x00000080: "Section contains uninitialised data (which will be initialised as 00 byte values before launch).",
    0x00000200: "Section contains comments for the linker.",
    0x00000800: "Section contents will not become part of image.",
    0x00001000: "Section contents comdat (Common Block Data).",
    0x00008000: "Section contents cannot be accessed relative to GP.",
    0x00100000: "Boundary alignment settings.",
    0x00200000: "Boundary alignment settings.",
    0x00400000: "Boundary alignment settings.",
    0x00800000: "Boundary alignment settings.",
    0x01000000: "Section contains extended relocations.",
    0x02000000: "Section can be discarded (e.g. .reloc).",
    0x04000000: "Section is not cacheable.",
    0x08000000: "Section is pageable.",
    0x10000000: "Section is shareable.",
    0x20000000: "<span class='important' >Section is executable.</span>",
    0x40000000: "<span class='important' >Section is readable.</span>",
    0x80000000: "<span class='important' >Section is writable.</span>"
}

_CHARACTERISTICS_FLAGS = {
    0x0001:  'IMAGE_FILE_RELOCS_STRIPPED Relocation info stripped from file.',
    0x0002:  '<span class="important">IMAGE_FILE_EXECUTABLE_IMAGE File is executable  (i.e. no unresolved externel references).</span>',
    0x0004:  'IMAGE_FILE_LINE_NUMS_STRIPPED Line nunbers stripped from file.',
    0x0008:  'IMAGE_FILE_LOCAL_SYMS_STRIPPED Local symbols stripped from file.',
    0x0010:  'IMAGE_FILE_AGGRESIVE_WS_TRIM Agressively trim working set',
    0x0020:  'IMAGE_FILE_LARGE_ADDRESS_AWARE App can handle >2gb addresses',
    0x0080:  'IMAGE_FILE_BYTES_REVERSED_LO Bytes of machine word are reversed.',
    0x0100:  'IMAGE_FILE_32BIT_MACHINE 32 bit word machine.',
    0x0200:  'IMAGE_FILE_DEBUG_STRIPPED Debugging info stripped from file in .DBG file',
    0x0400:  'IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP If Image is on removable media, copy and run from the swap file.',
    0x0800:  'IMAGE_FILE_NET_RUN_FROM_SWAP If Image is on Net, copy and run from the swap file.',
    0x1000:  'IMAGE_FILE_SYSTEM System File.',
    0x2000:  '<span class="important">IMAGE_FILE_DLL File is a DLL.</span>',
    0x4000:  'IMAGE_FILE_UP_SYSTEM_ONLY File should only be run on a UP machine',
    0x8000:  'IMAGE_FILE_BYTES_REVERSED_HI Bytes of machine word are reversed.',
}

_SUBSYSTEM = [
    'IMAGE_SUBSYSTEM_UNKNOWN Unknown subsystem.',
    'IMAGE_SUBSYSTEM_NATIVE Image doesn\'t require a subsystem.',
    'IMAGE_SUBSYSTEM_WINDOWS_GUI Image runs in the Windows GUI subsystem.',
    'IMAGE_SUBSYSTEM_WINDOWS_CUI Image runs in the Windows character subsystem.',
    'IMAGE_SUBSYSTEM_OS2_CUI image runs in the OS/2 character subsystem.',
    'IMAGE_SUBSYSTEM_POSIX_CUI image runs in the Posix character subsystem.',
    'IMAGE_SUBSYSTEM_NATIVE_WINDOWS image is a native Win9x driver.',
    'IMAGE_SUBSYSTEM_WINDOWS_CE_GUI Image runs in the Windows CE subsystem.',
    'IMAGE_SUBSYSTEM_EFI_APPLICATION',
    'IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER',
    'IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER',
    'IMAGE_SUBSYSTEM_EFI_ROM ',
    'IMAGE_SUBSYSTEM_XBOX ',
    'IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION',
]

def _splitReverse(strs):
    return ''.join([strs[:i][::-1] for i in range(0,len(strs)+4,4)])
        
def hex(value):
    if isinstance(value, str):
        return ''.join([("%02x" % ord(s)).upper() for s in _splitReverse(value)])
    elif isinstance(value, int) or isinstance(value, long):
        return ('%08x' % value).upper()
    return value

def hexFormat(value):
    """
    Translate object to hex format
    Arguments:
    - `value`:
    """

    if isinstance(value, int):
        return '0x'+("%08x" % value).upper()
    elif isinstance(value, long):
        return '0x'+("%08x" % value).upper()
    elif isinstance(value, str):
        if len(value) < 1:
            return '-'
        elif len(value) > 8:
            return value;

        return ''.join([value,'[0x', hex(value), ']'])
    else:
        return value
    
def sectionFlags(value):
    result = []
    for k in _SECTION_FLAGS.keys():
        if (k & value) > 0:
            result.append(_SECTION_FLAGS[k])
    return '<br />'.join(result)

def characteristicsFlag(value):
    result = []
    for k in _CHARACTERISTICS_FLAGS.keys():
        if (k & value) > 0:
            result.append(_CHARACTERISTICS_FLAGS[k])
    return '<br />'.join(result)


_MACHINES = {
    '014C': 'Intel 80386',
    '014D': 'Intel 80386',
    '014E': 'Intel 80386',
    '0160': 'R3000',
    '0162': 'R3000',
    '0166': 'R4000',
    '0168': 'R10000',
    '0184': 'DEC Alpha AXP10',
    '01F0': 'IBM Power PC'
}

def machine(value):
    """
    transalte string to machine.
    Arguments:
    - `value`:
    """
    return _MACHINES.get(hex(value),'Unknow machine')

def dataListLength(value):
    """
    Calc length of data list.
    Arguments:
    - `value`:
    """
    if isinstance(value, list):
        return sum(d.structLength for d in value)


def subsystem(value):
    """
    Show sub system.
    Arguments:
    - `value`:
    """
    if value < len(_SUBSYSTEM):
        return _SUBSYSTEM[value]
    return 'Unknow.'
    
register.filter('hexFormat', hexFormat)
register.filter('sectionFlags', sectionFlags)
register.filter('machine', machine)
register.filter('hex', hex)
register.filter('dataListLength', dataListLength)
register.filter('characteristicsFlag', characteristicsFlag)
register.filter('subsystem', subsystem)
