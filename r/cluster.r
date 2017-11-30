print("Hello r")

df <- read.csv("../data/weka/ecoli.csv", header = TRUE)

head(df)

table(df$class)

normalize <- function(x) {
  return ((x - min(x)) / (max(x) - min(x)))
}

purity <- function(clusters, classes) {
  sum(apply(table(classes, clusters), 2, max)) / length(clusters)
}

classes <- df$class
CLASS_SIZE <- length(table(classes))

df$class <- NULL
df <- as.data.frame(lapply(df, normalize))




dfCluster <- kmeans(df, CLASS_SIZE)

dfCluster


table(dfCluster$cluster, classes)
purity(dfCluster$cluster, classes)
library(cluster)
dfPAM <- pam( df, CLASS_SIZE )

dfPAM
table(dfPAM$clustering, classes)

purity(dfPAM$clustering, classes)

apply(table(dfPAM$cluster, classes),2,max)


