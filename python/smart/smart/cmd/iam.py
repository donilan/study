def execute(context):
    if context.param:
        context.connections.alias(context.client, context.param)
        return 'Now your name is [%s]' % context.param
    else:
        return 'Usage: %s aliasName' % context.cmdName
    
