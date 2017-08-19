#' @title
#' Type vector to topic indicator vector
#'
#' @description
#' Translate a vector of types to their topic indicators given a mode.
#'
#' @param types A vector of types.
#' @param mode A topictile mode.
#'
#' @export
type_to_topic_indicators <- function(types, mode) {
    mode$types_to_topic_indicators(types)
}

#' @title
#' Creates an empty topictile mode
#'
#' @description
#' Creates an empty topictile mode.
#'
#' @importFrom methods new
#'
#' @export
get_topictile_mode <- function() {
    module <- get_topictile_module()
    class <- module$TypeTopicIndicatorMode
    new(class)
}

#' @title
#' Creates a topictile mode given data
#'
#' @description
#' Creates a topictile mode given a type vector and a topic indicator vector.
#'
#' @param types A vector of types.
#' @param topic_indicators A vector of topic indicators.
#'
#' @export
get_topictile_mode_from_data <- function(types, topic_indicators) {
    mode <- get_topictile_mode()
    mode$update(types, topic_indicators)
    mode
}

#' @title
#' Creates a topictile mode given a topic model
#'
#' @description
#' Creates a topictile mode given a topic model that generates \code{gibbs_iters} samples.
#'
#' @param model A topic model.
#' @param gibbs_iters An integer specifying how many sampling to do.
#' @param dst A filename to where the model will write its samples temporarily.
#'
#' @export
get_topictile_mode_from_model <- function(model, gibbs_iters, dst) {
    mode <- get_topictile_mode()

    for (i in 1:gibbs_iters) {
        samples <- generate_samples_from(model, dst)
        mode$update(samples$token, samples$topic)
    }

    mode
}

#' @title
#' Writes a topictile mode to file
#'
#' @description
#' Writes a topictile mode to specified file location.
#'
#' @param mode A topictile mode.
#' @param dst A filename to where the mode data will be written.
#'
#' @importFrom utils write.table
#'
#' @export
write_topictile_mode_to_file <- function(mode, dst) {
    data <- mode$data()
    write.table(x=data, file=dst)
}

#' @title
#' Reads a topictile mode from file
#'
#' @description
#' Reads a topictile mode from specified file location.
#'
#' @param src A filename to where the mode data will be read.
#'
#' @importFrom utils read.table
#'
#' @export
read_topictile_mode_from_file <- function(src) {
    data <- read.table(src)
    get_topictile_mode_from_data(data$type, data$topic_indicator)
}

generate_samples_from <- function(model, filename) {
    if (file.exists(filename)) file.remove(filename)

    model$writeState(filename)
    tbl <- read.table(filename)
    names(tbl) <- c("doc", "source", "pos", "type", "token", "topic")

    if (file.exists(filename)) file.remove(filename)

    list(token=as.character(tbl$token), topic=as.numeric(tbl$topic))
}

get_topictile_module <- function() {
    Rcpp::Module("mod_topictile", "texmer")
}
