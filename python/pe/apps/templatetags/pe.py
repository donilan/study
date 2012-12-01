from django import template


register = template.Library()

_SECTION_FLAGS = {
    8: "Section should not be padded to next boundary.",
    20: "<span class='important' >Section contains code.</span>",
    40: "Section contains initialised data (which will become initialised with real values before the file is launched).",
    80: "Section contains uninitialised data (which will be initialised as 00 byte values before launch).",
    200: "Section contains comments for the linker.",
    800: "Section contents will not become part of image.",
    1000: "Section contents comdat (Common Block Data).",
    8000: "Section contents cannot be accessed relative to GP.",
    100000: "Boundary alignment settings.",
    200000: "Boundary alignment settings.",
    400000: "Boundary alignment settings.",
    800000: "Boundary alignment settings.",
    1000000: "Section contains extended relocations.",
    2000000: "Section can be discarded (e.g. .reloc).",
    4000000: "Section is not cacheable.",
    8000000: "Section is pageable.",
    10000000: "Section is shareable.",
    20000000: "<span class='important' >Section is executable.</span>",
    40000000: "<span class='important' >Section is readable.</span>",
    80000000: "<span class='important' >Section is writable.</span>"
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
