#!/usr/bin/env python2
from PIL import Image
import sys

tileheight=322
tilewidth=322

firstgid=21
Flip = True

for imgpath in sys.argv[1:]:  # prend le 1er arguments comme fichier
	img = Image.open(imgpath) #charge l'image
	img.load()
	width, height = img.size
	for j in xrange(0, height, tileheight):
		for i in xrange(0, width, tilewidth):
			im = img.crop((i, j, i + tilewidth, j + tileheight))
			im.load()
			if (Flip):
				im = im.transpose(Image.FLIP_TOP_BOTTOM)
			with open(imgpath + str(firstgid) + '.png', "wb") as out: # ouverture du fichier de sortie
				im.save(out) # sauvegarde
			firstgid += 1