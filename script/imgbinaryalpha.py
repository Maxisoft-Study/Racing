#!/usr/bin/env python2
from PIL import Image
import sys

for imgpath in sys.argv[1:]:  # lazy no check
	img = Image.open(imgpath)
	img.load()
	alpha = img.split()[-1]
	alphadata = alpha.getdata()
	alphadata = [pix if pix == 0 else 0xFF for pix in alphadata]
	alpha.putdata(alphadata)
	img.putalpha(alpha)

	with open(imgpath+'binaryalpha.png', "wb") as out:
		img.save(out)