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
cran_cec_log_path = '.'
gmum_cec_log_path = '.'
min_npoints = 100
max_npoints = 5000
npoints_step = 50
cran_max_iters = 200

gmum_cran_method_type_map = matrix(ncol=2, byrow=TRUE, 
                                   data=c('standard', 'all',
                                          'sphere', 'spherical',
                                          'diagonal', 'diagonal'))
method_type_num = dim(gmum_cran_method_type_map)[1]

valid_paths = 
    #(is_valid_path(cran_cec_path) == TRUE) &&
    (is_valid_path(gmum_cec_path) == TRUE) && 
    (is_valid_path(data_path, TRUE) == TRUE);

if(valid_paths == FALSE) {
    return;
}

get_gmum_result_file_path <- function(type_index)
{
    method_type = gmum_cran_method_type_map[type_index,][1]
    file_name = paste(c('gmum_', method_type, '.log'), collapse='')
    return(file.path(gmum_cec_log_path, file_name))
}

get_cran_result_file_path <- function(type_index)
{
    method_type = gmum_cran_method_type_map[type_index,][2]
    file_name = paste(c('cran_', method_type, '.log'), collapse='')
    return(file.path(cran_cec_log_path, file_name))
}
