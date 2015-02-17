#
# ## FAQ
#
# # How to get gmumr gource webm?
#
# Install dependencies:
#
# 1. Gource
# 2. FFMPEG
#
# Run `make` and wait :-)
#
# # How to edit movie captions?
#
# Movie captions are stored in `captions` file. Captions are divided to lines,
# and separated with pipe (`|`) symbol from UNIX timestamp:
#
# <UNIX_TIMESTAMP>|<CAPTION>
#
# # How to get UNIX timestamp from commit?
#
# `git show <COMMIT_SHA> --pretty=format:%at | head -n 1`
#
gmumr-gource.webm:
	gource --key -a 1 -s 0.1 --caption-file captions --stop-at-end \
	-1280x720 -o - | \
	ffmpeg -y -r 60 -f image2pipe -vcodec ppm -i - -vcodec libvpx \
	-b 20000K gmumr-gource.webm

