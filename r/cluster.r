library("clValid")
library("cluster")
library("clusterCrit")

#functions...
normalize <- function(x) {
  return ((x - min(x)) / (max(x) - min(x)))
}

purity <- function(clusters, classes) {
  return (sum(apply(table(classes, clusters), 2, max)) / length(clusters))
}

clusteringStats <- function(df, clusteringType) {

	classes <- df$class
	CLASS_COUNT <- length(table(classes))
#	print("CLASS COUNT: ")
#	str(CLASS_COUNT)
#	print("DATA FRAME: ")
	df$class <- NULL
#	str(df)
#	df <- as.data.frame(lapply(df, normalize))

	dfCluster <- clusteringType(df, CLASS_COUNT)

	intIdx <- intCriteria(as.matrix(df),dfCluster$cluster,c("Davies_Bouldin","Silhouette","Dunn"))

	value <- purity(dfCluster$cluster, classes)

	intIdx[["purity"]] <- value
	return (intIdx)
}

crossValidate <- function(df, clusteringType) {
results <- NULL
for (i in 1:10) 
{
  # These indices indicate the interval of the test set
  dataframe <- cbind(df)
  out <- split( df , f = dataframe$class )
  train <- NULL
  for (j in 1:length(table(dataframe$class)))
  {
    tmp <- out[[j]]
    indices <- (((i-1) * round((1/10)*nrow(tmp))) + 1):((i*round((1/10) * nrow(tmp))))
    # Exclude them from the train set
    train <- rbind( train, tmp[-indices,])
  }
  rownames(train) <- NULL

  results <- rbind( results, clusteringStats(train, kmeans) )
  clusteringStats(train, pam)
	
}
DBValue <- mean(as.double(results[,"davies_bouldin"]))
SValue <- mean(as.double(results[,"silhouette"]))
dunnValue <- mean(as.double(results[,"dunn"]))
purityValue <- mean(as.double(results[,"purity"]))

return( list( DBValue, SValue,dunnValue, purityValue) )
}

names <- list( "/home/data/seeds.csv", "/home/data/ecoli.csv", "/home/data/ukm.csv", "/home/data/ecoli.csv")
strings <- NULL
for (n in names)
{
df <- read.csv(n, header = TRUE)

out <- crossValidate(df, kmeans)
strings <- rbind( strings, sprintf("%s,%f,%f,%f,%f",n, out[[1]], out[[2]], out[[3]], out[[4]]) )
}

strings



