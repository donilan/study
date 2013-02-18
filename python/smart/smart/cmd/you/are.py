def execute(context):
    if context.isClient:
        context.client.alias = context.param
        return "I got it."
    return "I don't know how to do."
