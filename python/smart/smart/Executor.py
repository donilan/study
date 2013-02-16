
def serverExecute(addr, cmd):
    execute(addr, cmd, 'smart.cmd.server')

def execute(addr, cmd, basePackage='smart.cmd'):
    cmdName = cmd.split(' ')[0]
    exec 'import %s.%s as cmdModule' %(basePackage, cmdName)
    cmdModule.execute(addr, cmd[len(cmdName): ])
