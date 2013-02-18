import re, logging
import smart.const


def cmdArray2cmdName(cmdArr):
    if len(cmdArr) > 0:
        try:
            exec 'import %s.%s as cmdModule' % (smart.const.CMD_PACKAGE, '.'.join(cmdArr))
            return ' '.join(cmdArr)
        except ImportError:
            del cmdArr[-1]
            checkModule(cmdArr)
    else:
        return None

class Context(object):

    def __init__(self, cmd, log=logging.getLogger('Context')):
        self.target = ''
        self.cmdName = ''
        self.param = ''
        self.origin = ''
        self.badCommand = False
        self.log = log
        if cmd:
            tmpArr = re.compile(r'\s+').split(cmd)
            if len(tmpArr) > 1:
                self.target = tmpArr[0]
                del tmpArr[0]

                if tmpArr[-2] == 'for':
                    self.origin = tmpArr[-1]
                    cmd = re.sub('\\s*for\\s+%s\\s*$' % (self.origin), '', cmd)
                    del tmpArr[-2:]

                self.cmdName = self.__cmdArray2cmdName(tmpArr)
                if self.cmdName:
                    self.log.debug('Command is %s' % self.cmdName)
                    self.param = re.sub('^\\s*' + \
                                            '\\s+'.join([self.target,] \
                                                            + self.cmdName.split(' ')) + '\\s+', '', cmd)
                else:
                    self.log.warn('Bad command [%s]' % cmd)
                    self.badCommand = True
                    self.cmdName = 'log error'
                    self.param = cmd

    def __cmdArray2cmdName(self, cmdArr):
        if len(cmdArr) > 0:
            try:
                exec 'import %s.%s as cmdModule' % (smart.const.CMD_PACKAGE, '.'.join(cmdArr))
                return ' '.join(cmdArr)
            except:
                del cmdArr[-1]
                return self.__cmdArray2cmdName(cmdArr)
        else:
            return None

    def cmd(self):
        cmd = [self.target, self.cmdName, self.param]
        if self.origin:
            cmd += ['for', self.origin]
        return ' '.join(cmd)

    def isClient(self):
        return hasattr(self, 'client')

    def isServer(self):
        return hasattr(self, 'server')

class CommandExecutor(object):

    def __init__(self, context):
        self.context = context

    def run(self):
        if self.context.cmdName:
            try:
                exec 'import %s.%s as cmdModule' % (smart.const.CMD_PACKAGE, self.context.cmdName.replace(' ', '.'))
            except:
                return '%s Don\'t know how to execute [%s]' % \
                    (self.context.cmd(), self.context.cmdName)
            return cmdModule.execute(self.context)
        else:
            return 'Error: Command name cannot be empty.'

