from django import template


register = template.Library()


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
    

register.filter('hexFormat', hexFormat)
