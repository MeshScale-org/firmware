
Do we have a single processor/core or multicore?

For single core:
    use cooperative threading
    threads must release all resources before returning to scheduler
    no locks are needed
    interrupts may not use resources

For multiCore:
    use of freeRTOS (ESP-IDF or vanilla)
    a thread can be interrupted
    locks are used
    interrupts may not use resources

