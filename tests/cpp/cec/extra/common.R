is_valid_path <- function(path, create_path=FALSE) {
    if(file.exists(path) == FALSE)
    {
        print(paste(c(path, "path is wrong"), collapse=' '))
        if(create_path == TRUE) {
            dir.create(path)
            return(TRUE)
        }
        return(FALSE)
    }
    return(TRUE)
}

#cran_cec_path = '../../../../../cran-cec'
gmum_cec_path = '../../../../'
data_path = './data/'
cran_cec_log_path = './cran_cec.log'
gmum_cec_log_path = './gmum_cec.log'
min_npoints = 10000
max_npoints = 20000
npoints_step = 100
cran_max_iters = 200

valid_paths = 
    #(is_valid_path(cran_cec_path) == TRUE) &&
    (is_valid_path(gmum_cec_path) == TRUE) && 
    (is_valid_path(data_path, TRUE) == TRUE);

if(valid_paths == FALSE) {
    return;
}
