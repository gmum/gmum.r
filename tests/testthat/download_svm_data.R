svm.data.root.local <- file.path("inst", "dev", "datasets", "svm") 
colon_cancer.filename <- "colon-cancer"
colon_cancer.url <- "http://www.csie.ntu.edu.tw/~cjlin/libsvmtools/datasets/binary/colon-cancer.bz2" 

svm.download.colon_cancer <- function() {
	svm.download.dataset(colon_cancer.url, colon_cancer.filename)	
}

#right now is working only wiz bzip2
svm.download.dataset <- function(url, filename, root=svm.data.root.local) {
	destfile <- file.path(root,filename)
	if(!file.exists(destfile)) {
		filename_download <- basename(url) 
		ext <- strsplit(filename_download, "\\.")[[1]][2]
		ext <- switch(ext,
		       bz2 = "bzip2")
		destfile_download <- file.path(root,filename_download)
		download.file(url,destfile=destfile_download)	
		destfile_con <- file(destfile, "w")
		destfile_download_con <- bzfile(destfile_download)
		writeLines(readLines(destfile_download_con, warn=FALSE), destfile_con) 
		unlink(destfile_download)
		close(destfile_con)
		close(destfile_download_con)
	} else {
	 	print("Everything ok");
	}
}

svm.download.colon_cancer()
