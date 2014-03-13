#!/usr/bin/env python2
from PIL import Image
import sys

tileheight=322
tilewidth=322

firstgid=21

for imgpath in sys.argv[1:]:  # prend le 1er arguments comme fichier
	img = Image.open(imgpath) #charge l'image
	img.load()
	width, height = img.size

	for i in xrange(0, width, tilewidth):
		for j in xrange(0, height, tileheight):
			im = img.crop((i, j, i + tilewidth, j + tileheight))
			im.load()
			with open(imgpath + str(firstgid) + '.png', "wb") as out: # ouverture du fichier de sortie
				im.save(out) # sauvegarde
			firstgid += 1