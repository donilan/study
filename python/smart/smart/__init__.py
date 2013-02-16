import re
class Context(object):

    def __init__(self, cmd):
        self.target = ''
        self.cmdName = ''
        self.param = ''
        self.orgCmd = cmd
        if cmd:
            tmpArr = re.compile(r'\s+').split(cmd)
            if len(tmpArr) > 1:
                self.target = tmpArr[0]
                self.cmdName = tmpArr[1]
                self.param = re.sub('%s\\s+%s\\s*' % (self.target, 
                                                    self.cmdName), '', cmd)


class CommandExecutor(object):

    def __init__(self, context):
        self.context = context

    def run(self):
        if self.context.cmdName:
            exec 'import smart.cmd.%s as cmdModule' % (self.context.cmdName)
            return cmdModule.execute(self.context)
        else:
            return 'Error: Command name cannot be empty.'

