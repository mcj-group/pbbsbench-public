// This code is part of the Problem Based Benchmark Suite (PBBS)
// Copyright (c) 2011 Guy Blelloch and the PBBS team
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights (to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <algorithm>
#include "parlay/parallel.h"
#include "parlay/primitives.h"
#include "parlay/random.h"
#include "common/geometry.h"
#include "../utils/NSGDist.h"  
#include "../utils/types.h"
#include "pynn_index2.h"
#include "../utils/beamSearch.h"  
#include "../utils/indexTools.h"
#include "../utils/stats.h"

extern bool report_stats;

template<typename T>
void ANN(parlay::sequence<Tvec_point<T>*> &v, int k, int K, int cluster_size, int beamSizeQ, double num_clusters, double delta,
  parlay::sequence<Tvec_point<T>*> &q) {
  parlay::internal::timer t("ANN",report_stats); 
  {
    unsigned d = (v[0]->coordinates).size();
    using findex = pyNN_index<T>;
    findex I(K, d, delta);
    I.build_index(v, cluster_size);
    t.next("Built index");
    beamSearchRandom(q, v, beamSizeQ, k, d);
    t.next("Found nearest neighbors");
    if(report_stats){
      //average numbers of nodes searched using beam search
      graph_stats(v);
      query_stats(q);
      t.next("stats");
    }
  };
}


template<typename T>
void ANN(parlay::sequence<Tvec_point<T>*> v, int K, int cluster_size, double num_clusters, double delta) {
  parlay::internal::timer t("ANN",report_stats); 
  { 
    unsigned d = (v[0]->coordinates).size();
    using findex = pyNN_index<T>;
    findex I(K, d, delta);
    I.build_index(v, cluster_size);
    t.next("Built index");
    if(report_stats){
      graph_stats(v);
      t.next("stats");
    }
  };
}
