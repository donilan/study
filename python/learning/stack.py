#!/usr/bin/env python

stack = []

def pushit():
    stack.append(raw_input(' Enter New string: ').strip())

def popit():
    if len(stack) == 0:
        print 'Cannot pop from an empty stack!'
    else:
        print 'Remove [', `stack.pop()`, ']'

def viewstack():
    print stack

CMSs = {'u': pushit, 'o': popit, 'v': viewstack}

def showmenu():
    pr="""
P(U)sh
P(O)p
(V)iew
(Q)uit

Enter choice: """

    while True:
        while True:
            try:
                choice = raw_input(pr).strip()[0].lower()
            except (OEFError, KeyboardInterrupt, IndexError):
                choise = 'q'
                
            print '\nYou picked: [%s]' % choice
            if choice not in 'uovq':
                print 'Invalid option, try again'
            else:
                break

        if choice == 'q':
            break
        CMSs[choice]()

if __name__ == '__main__':
    showmenu()
