
def execute(context):
    result = """
------- Client list -------
%s
---- Client list end ------
"""
    return result % ('\n'.join('client: %s' % k 
                         for k, v in context.connections.items()))
    

        
