
def execute(context):
    return '\n'.join('client: %s' % k 
                         for k, v in context.connections.items())
    

        
