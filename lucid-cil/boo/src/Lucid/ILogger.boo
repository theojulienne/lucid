namespace Lucid

import System


interface ILogger:
    def WriteDebug(message as string, args as (object))
    def WriteWarning(message as string, args as (object))

