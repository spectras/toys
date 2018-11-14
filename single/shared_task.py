import asyncio
from contextlib import suppress


class SharedTask:
    ''' Allow multiple tasks to await a common sub-task.

    Awaiting tasks can be cancelled independently. Cancelling all tasks
    will cancel the sub-task.

    :param task: the sub-task. Either a co-routine or a pending Future.
    '''
    __slots__ = ('_clients', '_task')

    def __init__(self, task):
        if not (asyncio.isfuture(task) or asyncio.iscoroutine(task)):
            raise TypeError('task must be either a Future or a coroutine object')
        self._clients = 0
        self._task = task

    @property
    def started(self):
        ''' Whether the underlying task has been started '''
        return asyncio.isfuture(self._task)

    async def wait(self, *, timeout=None):
        ''' Start task if it did not start yet, and wait until it completes.

        :param timeout: time, in seconds, after which waiting should be interrupted.
                        Note this will not cancel the subtask, unless this task is the last
                        one awaiting it.
        :return: whatever the sub-task returns.
        :raises asyncio.CancelledError: if either waiting or sub-task is cancelled.
        :raises asyncio.TimeoutError: if timeout is reached.
        :raises Exception: any exception raised by sub-task is re-raised.
        '''
        self._task = asyncio.ensure_future(self._task)

        self._clients += 1
        try:
            return await asyncio.wait_for(asyncio.shield(self._task), timeout=timeout)
        except:
            self._clients -= 1
            if self._clients == 0 and not self._task.done():
                self._task.cancel()
                with suppress(asyncio.CancelledError):
                    await self._task
            raise

    def cancel(self):
        ''' Cancel shared task unconditionally '''
        if asyncio.iscoroutine(self._task):
            self._task.close()      # avoid 'coroutine was never awaited' warning
        elif asyncio.isfuture(self._task):
            self._task.cancel()
