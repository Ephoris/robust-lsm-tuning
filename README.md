# Endure: A Robust Tuning Paradigm for LSM Trees Under Workload Uncertainty
  

Log-structured merge-trees (LSM trees) are increasingly used as the storage
engines behind several data systems, many of which are deployed in the cloud.
Similar to other database architectures, LSM trees take into account information
about the expected workloads (e.g., reads vs. writes and point vs. range
queries) and optimize their performances by changing tunings. Operating in the
cloud, however, comes with a degree of uncertainty due to multi-tenancy and the
fast-evolving nature of modern applications. Databases with static tunings
discount the variability of such hybrid workloads and hence provide an
inconsistent and overall suboptimal performance.


To address this problem, we introduce Endure – a new paradigm for tuning LSM
Trees in the presence of workload uncertainty. Specifically, we focus on the
impact of the choice of compaction policies, size-ratio, and memory allocation
on the overall query performance. Endure considers a robust formulation of the
throughput maximization problem, and recommends a tuning that maximizes the
worst-case throughput over the neighborhood of an expected workload.
Additionally, an uncertainty tuning parameter controls the size of this
neighborhood, thereby allowing the output tunings to be conservative or
optimistic. We benchmark Endure on a state-of-the-art LSM-based storage engine,
RocksDB, and show that its tunings comprehensively outperform tunings from
classical strategies. Drawing upon the results of our extensive analytical and
empirical evaluation, we recommend the use of Endure for optimizing the
performance of LSM tree-based storage engines.


# Running Endure


In this code package we provide a couple different ways to explore Endure.

1. in `tools/robust-lsm-trees` we provide Python scripts that we utilize for