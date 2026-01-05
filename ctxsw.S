# context swithcing implementation

.global ctxsw

ctxsw:
    # save current stack pointer
    movl 4(%esp), %eax
    movl %esp, (%eax)

    # load new stack pointer
    movl 8(%esp), %esp

    ret