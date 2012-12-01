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

def _splitReverse(strs):
    return ''.join([strs[:i][::-1] for i in range(0,len(strs)+4,4)])
        
def str2hex(strs):
    return ''.join([("%02x" % ord(s)).upper() for s in _splitReverse(strs)])

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

        return ''.join([value,'[0x', str2hex(value), ']'])
    else:
        return value
    
def sectionFlags(value):
    result = []
    for k in _SECTION_FLAGS.keys():
        if (k & value) > 0:
            result.append(_SECTION_FLAGS[k])
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
    return _MACHINES.get(str2hex(value),'Unknow machine')

register.filter('hexFormat', hexFormat)
register.filter('sectionFlags', sectionFlags)
register.filter('machine', machine)
register.filter('hex', str2hex)
