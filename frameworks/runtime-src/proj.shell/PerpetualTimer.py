from threading import Timer,Thread,Event
class PerpetualTimer():
    def __init__(self,t,hFunction):
        self.t = t
        self.is_cancel = False
        self.hFunction = hFunction
        self.thread = Timer(self.t,self.handle_function)
    def handle_function(self):
        if(self.is_cancel):
            return
        self.hFunction()
        self.thread = Timer(self.t,self.handle_function)
        self.thread.start()
    def start(self):
        self.thread.start()
    def cancel(self):
        self.thread.cancel()
        self.is_cancel = True






