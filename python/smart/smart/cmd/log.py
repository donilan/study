
def execute(context):
    if not context.param:
        return 'USAGE %s LEVEL' % context.cmdName
    level = context.param.split(' ')[0]
    msg = context.param[len(level)+1: ]
    print ' -- Smart log:', level, '-',  msg
