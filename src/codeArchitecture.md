

Managers run in a thread, handlers only run when called (by a manager).

Managers interact with eachother using (thread safe) message queues, managerSystem is the main manager that spreads incoming messages to other managers

Handlers can be called by multiple managers and must inherit a resourceLock (handler_base). Handler locks itself when called

All other objects and classes should be kept dedicated to a single handler as much as possible.
(e.g. all interface/radio classes are only called by handlerNetworkInterfaces)

Objects where this is not possible must also inherit from resourceLock and should be locked before use
(e.g. SPI, I2C, filesystem, ...)

