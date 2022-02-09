/**
 *  The Kendall's distance rank similarity metric.
 *
 *  The KD metric is described in:
 *
 *  @article{fks03:siam,
 *      author = {Ronald Fagin and Ravi Kumar and D. Sivakumar},
 *      title = {Comparing top $k$ lists},
 *      journal = {SIAM Journal on Discrete Mathematics},
 *      volume = {17},
 *      number = {1},
 *      year = {2003},
 *      pages = {134--160},
 *  }
 *
 *  Fagin et al. define the measure to range between -1 (disjoint)
 *  and 1 (identical), that is, as a similarity measure,  In their
 *  experiments, though, they normalize and invert it to range 
 *  between 1 (disjoint) to 0 (identical); that is, it becomes a
 *  distance metric.  The implementation here uses the original,
 *  similarity definition.
 *
 *  XXX I will implement Kendall's $tau$ first.
 */
