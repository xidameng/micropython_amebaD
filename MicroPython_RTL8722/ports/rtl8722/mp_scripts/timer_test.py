t = Timer(0)
t.init()

def fun():
	print("timer fired")

t.start(1000000, fun(), t.PERIODICAL)