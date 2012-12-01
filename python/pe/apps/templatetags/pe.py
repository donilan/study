from django import template


register = template.Library()

_SECTION_FLAGS = {
    8: "Section should not be padded to next boundary",
    20: "Section contains code",
    40: "Section contains initialised data (which will become initialised with real values before the file is launched)",
    80: "Section contains uninitialised data (which will be initialised as 00 byte values before launch)",
    200: "Section contains comments for the linker",
    800: "Section contents will not become part of image",
    1000: "Section contents comdat (Common Block Data)",
    8000: "Section contents cannot be accessed relative to GP",
    100000: "Boundary alignment settings",
    200000: "Boundary alignment settings",
    400000: "Boundary alignment settings",
    800000: "Boundary alignment settings",
    1000000: "Section contains extended relocations",
    2000000: "Section can be discarded (e.g. .reloc)",
    4000000: "Section is not cacheable",
    8000000: "Section is pageable",
    10000000: "Section is shareable",
    20000000: "Section is executable",
    40000000: "Section is readable",
    80000000: "Section is writable"
}

def hexFormat(value):
    """
    Translate object to hex format
    Arguments:
    - `value`:
    """

    if isinstance(value, int):
        return '0x'+("%04x" % value).upper()
    elif isinstance(value, long):
        return '0x'+("%08x" % value).upper()
    elif isinstance(value, str):
        if len(value) < 1:
            return '-'
        return ''.join([value,'[0x']+[("%02x" % ord(s)).upper() for s in value]+[']'])
    else:
        return value
    
def sectionFlags(value):
    result = []
    for k in _SECTION_FLAGS.keys():
        if (k & value) > 0:
            result.append(_SECTION_FLAGS[k])
    return '<br />'.join(result)

register.filter('hexFormat', hexFormat)
register.filter('sectionFlags', sectionFlags)
