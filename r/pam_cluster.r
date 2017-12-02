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

clusteringStats <- function(df, clusteringType, cols, clu_num, algo) {

	classes <- df$class
	CLASS_COUNT <- clu_num
#	print("CLASS COUNT: ")
#	str(CLASS_COUNT)
#	print("DATA FRAME: ")
	df$class <- NULL
#	str(df)
#	df <- as.data.frame(lapply(df, normalize))

	dfCluster <- clusteringType(df[,1:cols], CLASS_COUNT, metric = algo)

	intIdx <- intCriteria(as.matrix(df),dfCluster$cluster,c("Davies_Bouldin","Silhouette","Dunn"))

	value <- purity(dfCluster$cluster, classes)

	intIdx[["purity"]] <- value
	return (intIdx)
}

crossValidate <- function(df, clusteringType, cols, clu_num, algo) {
results <- NULL
for (i in 1:10) 
{
  # These indices indicate the interval of the test set
  df <- df[sample(nrow(df)),]
  out <- split( df , f = df$class )
  train <- NULL
    indices <- (((i-1) * round((1/10)*nrow(df))) + 1):((i*round((1/10) * nrow(df))))
    # Exclude them from the train set
    train <- rbind( train, df[-indices,])
  rownames(train) <- NULL

  res <- clusteringStats(train, clusteringType, cols, clu_num, algo)
  res[["cross"]] <- i

  results <- rbind( results, res )	
}
DBValue <- mean(as.double(results[,"davies_bouldin"]))
SValue <- mean(as.double(results[,"silhouette"]))
dunnValue <- mean(as.double(results[,"dunn"]))
purityValue <- mean(as.double(results[,"purity"]))

return( results)
}

names <- list( "/home/data/seeds.csv", "/home/data/ecoli.csv", "/home/data/ukm.csv", "/home/data/customers.csv")
strings <- NULL
for (n in names)
{
  df <- read.csv(n, header = TRUE)

  for ( cols in 1:length(df)-1 )
  {
    for ( clu_num in 1:10 )
    {
	for ( a in list("manhattan","euclidean") )
	{
	  res <- crossValidate(df, pam, cols, clu_num, a)
	  for( row_id in 1:nrow(res) )
	  {
	    strings <- rbind( strings, sprintf("%s,%s,cross_%1.0f,%1.0f,%1.0f,%f,%f,%f,%f",n,a,res[row_id,"cross"],cols,clu_num, res[row_id,"davies_bouldin"], res[row_id,"silhouette"], res[row_id,"dunn"], res[row_id,"purity"]) )
	  }
	}
    }
  }
}

write.table( strings, "pam.txt" )



