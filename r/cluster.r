library("clValid")
library("cluster")
library("clusterCrit")

#functions...
normalize <- function(x) {
  return ((x - min(x)) / (max(x) - min(x)))
}

purity <- function(clusters, classes) {
  return (sum(apply(table(classes, clusters), 2, max)) / length(clusters))[1]
}

clusteringStats <- function(df, clusteringType) {

	classes <- df$class
	CLASS_SIZE <- length(table(classes))

	df$class <- NULL
	df <- as.data.frame(lapply(df, normalize))

	dfCluster <- clusteringType(df, CLASS_SIZE)

	intIdx <- intCriteria(as.matrix(df),dfCluster$cluster,c("Silhouette","Dunn"))

	value <- purity(dfCluster$cluster, classes)

	intIdx[["purity"]] <- value

	return (intIdx)
}

df <- read.csv("/home/data/ecoli.csv", header = TRUE)

clusteringStats(df, pam)
clusteringStats(df, kmeans)
