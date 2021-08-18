try:
    import board
    import neopixel
    pixels = neopixel.NeoPixel(board.D18, 102)
    pixels.auto_write = False

except NotImplementedError:
    pass

def setPixel(p,r,g,b):
        pixels[p] = (r,g,b)
        print("Hallo")

def ledupdate():
    pixels.show()

