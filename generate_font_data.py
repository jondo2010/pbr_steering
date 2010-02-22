# -*- coding: utf-8 -*-

img = gimp.image_list()[0]
numchars = img.width / 16

listA = []
listB = []
for cn in range(numchars):
	rgn = img.active_layer.get_pixel_rgn(cn*16,0,16,16)
	for i in range(16):
		byteA = int()
		byteB = int()
		for j in range(16):
			if rgn[rgn.x+j,rgn.y+i] == '\x01':
				if j < 8:
					byteA |= (1 << 7-(j%8))
				else:
					byteB |= (1 << 7-(j%8))
		listA.append(byteA)
		listB.append(byteB)


strlistA = []
strlistB = []
for c in listA:
	strlistA.append('0x%02X'%c)

for c in listB:
	strlistB.append('0x%02X'%c)

strA = "{" + ", ".join(strlistA) + "}"
strB = "{" + ", ".join(strlistB) + "}"
