rm(list=ls())

#EllipseGauss
cec_ellipse_gauss.energy <- as.numeric(read.table(system.file("data_sets", "cec", "EllipseGauss","energy.txt", package="gmum.r")));
cec_ellipse_gauss.input <- as.matrix(read.table(system.file("data_sets", "cec", "EllipseGauss","input.txt", package="gmum.r")));
cec_ellipse_gauss.cluster <- as.vector(read.table(system.file("data_sets", "cec", "EllipseGauss","cluster.txt", package="gmum.r")));

typeof(cec_ellipse_gauss.energy);
typeof(cec_ellipse_gauss.input);
typeof(cec_ellipse_gauss.cluster);

save.image("cec_ellipse_gauss.RData");

env <- c(ls())
rm(env, list = env)

#mouse_1
cec_mouse_1.energy <- as.numeric(read.table(system.file("data_sets", "cec", "mouse_1","energy.txt", package="gmum.r")));
cec_mouse_1.input <- as.matrix(read.table(system.file("data_sets", "cec", "mouse_1","input.txt", package="gmum.r")));
cec_mouse_1.cluster <- as.vector(read.table(system.file("data_sets", "cec", "mouse_1","cluster.txt", package="gmum.r")));

typeof(cec_mouse_1.energy);
typeof(cec_mouse_1.input);
typeof(cec_mouse_1.cluster);

save.image("cec_mouse_1.RData");

env <- c(ls())
rm(env, list = env)

#mouse_1_classic
cec_mouse_1_classic.energy <- as.numeric(read.table(system.file("data_sets", "cec", "mouse_1_classic","energy.txt", package="gmum.r")));
cec_mouse_1_classic.input <- as.matrix(read.table(system.file("data_sets", "cec", "mouse_1_classic","input.txt", package="gmum.r")));
cec_mouse_1_classic.cluster <- as.vector(read.table(system.file("data_sets", "cec", "mouse_1_classic","cluster.txt", package="gmum.r")));

cec_mouse_1_classic.aproximation <- as.numeric(read.table(system.file("data_sets", "cec", "mouse_1_classic","aproximation.txt", package="gmum.r")));
cec_mouse_1_classic.dimension <- as.numeric(read.table(system.file("data_sets", "cec", "mouse_1_classic","dimension.txt", package="gmum.r")));
cec_mouse_1_classic.iteration <- as.numeric(read.table(system.file("data_sets", "cec", "mouse_1_classic","iteration.txt", package="gmum.r")));
cec_mouse_1_classic.type <- read.table(system.file("data_sets", "cec", "mouse_1_classic","type.txt", package="gmum.r"));

typeof(cec_mouse_1_classic.energy);
typeof(cec_mouse_1_classic.input);
typeof(cec_mouse_1_classic.cluster);

typeof(cec_mouse_1_classic.aproximation);
typeof(cec_mouse_1_classic.dimension);
typeof(cec_mouse_1_classic.iteration);
typeof(cec_mouse_1_classic.type);

save.image("cec_mouse_1_classic.RData");

env <- c(ls())
rm(env, list = env)

#mouse_1_spherical
cec_mouse_1_spherical.energy <- as.numeric(read.table(system.file("data_sets", "cec", "mouse_1_spherical","energy.txt", package="gmum.r")));
cec_mouse_1_spherical.input <- as.matrix(read.table(system.file("data_sets", "cec", "mouse_1_spherical","input.txt", package="gmum.r")));
cec_mouse_1_spherical.cluster <- as.vector(read.table(system.file("data_sets", "cec", "mouse_1_spherical","cluster.txt", package="gmum.r")));

cec_mouse_1_spherical.aproximation <- as.numeric(read.table(system.file("data_sets", "cec", "mouse_1_spherical","aproximation.txt", package="gmum.r")));
cec_mouse_1_spherical.dimension <- as.numeric(read.table(system.file("data_sets", "cec", "mouse_1_spherical","dimension.txt", package="gmum.r")));
cec_mouse_1_spherical.iteration <- as.numeric(read.table(system.file("data_sets", "cec", "mouse_1_spherical","iteration.txt", package="gmum.r")));
cec_mouse_1_spherical.type <- read.table(system.file("data_sets", "cec", "mouse_1_spherical","type.txt", package="gmum.r"));

typeof(cec_mouse_1_spherical.energy);
typeof(cec_mouse_1_spherical.input);
typeof(cec_mouse_1_spherical.cluster);

typeof(cec_mouse_1_spherical.aproximation);
typeof(cec_mouse_1_spherical.dimension);
typeof(cec_mouse_1_spherical.iteration);
typeof(cec_mouse_1_spherical.type);

save.image("cec_mouse_1_spherical.RData");

env <- c(ls())
rm(env, list = env)

#mouse_2_spherical
cec_mouse_2_spherical.energy <- as.numeric(read.table(system.file("data_sets", "cec", "mouse_2_spherical","energy.txt", package="gmum.r")));
cec_mouse_2_spherical.input <- as.matrix(read.table(system.file("data_sets", "cec", "mouse_2_spherical","input.txt", package="gmum.r")));
cec_mouse_2_spherical.cluster <- as.vector(read.table(system.file("data_sets", "cec", "mouse_2_spherical","cluster.txt", package="gmum.r")));

cec_mouse_2_spherical.aproximation <- as.numeric(read.table(system.file("data_sets", "cec", "mouse_2_spherical","aproximation.txt", package="gmum.r")));
cec_mouse_2_spherical.dimension <- as.numeric(read.table(system.file("data_sets", "cec", "mouse_2_spherical","dimension.txt", package="gmum.r")));
cec_mouse_2_spherical.iteration <- as.numeric(read.table(system.file("data_sets", "cec", "mouse_2_spherical","iteration.txt", package="gmum.r")));
cec_mouse_2_spherical.type <- read.table(system.file("data_sets", "cec", "mouse_2_spherical","type.txt", package="gmum.r"));

typeof(cec_mouse_2_spherical.energy);
typeof(cec_mouse_2_spherical.input);
typeof(cec_mouse_2_spherical.cluster);

typeof(cec_mouse_2_spherical.aproximation);
typeof(cec_mouse_2_spherical.dimension);
typeof(cec_mouse_2_spherical.iteration);
typeof(cec_mouse_2_spherical.type);

save.image("cec_mouse_2_spherical.RData");

env <- c(ls())
rm(env, list = env)

#simple_1
cec_simple_1.energy <- as.numeric(read.table(system.file("data_sets", "cec", "simple_1","energy.txt", package="gmum.r")));
cec_simple_1.input <- as.matrix(read.table(system.file("data_sets", "cec", "simple_1","input.txt", package="gmum.r")));
cec_simple_1.cluster <- as.vector(read.table(system.file("data_sets", "cec", "simple_1","cluster.txt", package="gmum.r")));

typeof(cec_simple_1.energy);
typeof(cec_simple_1.input);
typeof(cec_simple_1.cluster);

save.image("cec_simple_1.RData");

env <- c(ls())
rm(env, list = env)
