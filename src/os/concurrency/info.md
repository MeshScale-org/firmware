
Do we have a single processor/core or multicore?

For single core:
    threads must release all resources before returning to scheduler
    no locks are needed, but implemented for good practice
    interrupts may not use resources, only volatile vars

For multiCore:
    use of an RTOS
    a thread can be interrupted
    locks are used
    interrupts may not use resources, only volatile vars

