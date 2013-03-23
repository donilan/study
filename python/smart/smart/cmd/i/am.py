
def execute(context):
    if context.isServer and context.param:
        context.connections.alias(context.origin, context.param)
        context.connections.send('%s you are %s' % (context.origin, context.param))
        return 'Now your name is [%s]' % context.param
    else:
        return 'Usage: %s aliasName' % context.cmdName
    
