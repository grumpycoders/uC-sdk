#include <stdlib.h>

unsigned int _seed = 2891583007UL;

int rand_r(unsigned int * seed) {
    unsigned int a = *seed;
    a ^= 61;
    a ^= a >> 16;
    a += a << 3;
    a ^= a >> 4;
    a *= 668265263UL;
    a ^= a >> 15;
    a *= 3148259783UL;
    *seed = a;
    return (int) a;
}

#ifdef RAND_TEST

#include <stdio.h>

int main() {
    int e = 1;

    while (e == 1) {
        int r = rand_r(&_seed);
        e = fwrite(&r, sizeof(r), 1, stdout);
    }
}

/*

$ dieharder -l
#=============================================================================#
#            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
#=============================================================================#
Installed dieharder tests:
 Test Number                         Test Name                Test Reliability
===============================================================================
  -d 0                            Diehard Birthdays Test              Good
  -d 1                               Diehard OPERM5 Test              Good
  -d 2                    Diehard 32x32 Binary Rank Test              Good
  -d 3                      Diehard 6x8 Binary Rank Test              Good
  -d 4                            Diehard Bitstream Test              Good
  -d 5                                      Diehard OPSO           Suspect
  -d 6                                 Diehard OQSO Test           Suspect
  -d 7                                  Diehard DNA Test           Suspect
  -d 8                Diehard Count the 1s (stream) Test              Good
  -d 9                  Diehard Count the 1s Test (byte)              Good
  -d 10                         Diehard Parking Lot Test              Good
  -d 11         Diehard Minimum Distance (2d Circle) Test             Good
  -d 12         Diehard 3d Sphere (Minimum Distance) Test             Good
  -d 13                             Diehard Squeeze Test              Good
  -d 14                                Diehard Sums Test        Do Not Use
  -d 15                                Diehard Runs Test              Good
  -d 16                               Diehard Craps Test              Good
  -d 17                     Marsaglia and Tsang GCD Test              Good
  -d 100                                STS Monobit Test              Good
  -d 101                                   STS Runs Test              Good
  -d 102                   STS Serial Test (Generalized)              Good
  -d 200                       RGB Bit Distribution Test              Good
  -d 201           RGB Generalized Minimum Distance Test              Good
  -d 202                           RGB Permutations Test              Good
  -d 203                             RGB Lagged Sum Test              Good
  -d 204                RGB Kolmogorov-Smirnov Test Test              Good
  -d 205                               Byte Distribution              Good
  -d 206                                         DAB DCT              Good
  -d 207                              DAB Fill Tree Test              Good
  -d 208                            DAB Fill Tree 2 Test              Good
  -d 209                              DAB Monobit 2 Test              Good
$ ./rand | dieharder -g 200 -a
#=============================================================================#
#            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
#=============================================================================#
   rng_name    |rands/second|   Seed   |
stdin_input_raw|  5.48e+07  |2658615066|
#=============================================================================#
        test_name   |ntup| tsamples |psamples|  p-value |Assessment
#=============================================================================#
   diehard_birthdays|   0|       100|     100|0.34598849|  PASSED
      diehard_operm5|   0|   1000000|     100|0.88561246|  PASSED
  diehard_rank_32x32|   0|     40000|     100|0.55118719|  PASSED
    diehard_rank_6x8|   0|    100000|     100|0.76080546|  PASSED
   diehard_bitstream|   0|   2097152|     100|0.93596325|  PASSED
        diehard_opso|   0|   2097152|     100|0.00000000|  FAILED
        diehard_oqso|   0|   2097152|     100|0.31689021|  PASSED
         diehard_dna|   0|   2097152|     100|0.25362336|  PASSED
diehard_count_1s_str|   0|    256000|     100|0.78572174|  PASSED
diehard_count_1s_byt|   0|    256000|     100|0.58373769|  PASSED
 diehard_parking_lot|   0|     12000|     100|0.25460871|  PASSED
    diehard_2dsphere|   2|      8000|     100|0.75310619|  PASSED
    diehard_3dsphere|   3|      4000|     100|0.22288967|  PASSED
     diehard_squeeze|   0|    100000|     100|0.82973282|  PASSED
        diehard_sums|   0|       100|     100|0.00233546|   WEAK
        diehard_runs|   0|    100000|     100|0.60997475|  PASSED
        diehard_runs|   0|    100000|     100|0.28281230|  PASSED
       diehard_craps|   0|    200000|     100|0.35086666|  PASSED
       diehard_craps|   0|    200000|     100|0.25516065|  PASSED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.09354271|  PASSED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.11957036|  PASSED
         sts_monobit|   1|    100000|     100|0.89024038|  PASSED
            sts_runs|   2|    100000|     100|0.84144147|  PASSED
          sts_serial|   1|    100000|     100|0.31131119|  PASSED
          sts_serial|   2|    100000|     100|0.27269354|  PASSED
          sts_serial|   3|    100000|     100|0.17496017|  PASSED
          sts_serial|   3|    100000|     100|0.36121271|  PASSED
          sts_serial|   4|    100000|     100|0.80701227|  PASSED
          sts_serial|   4|    100000|     100|0.19967722|  PASSED
          sts_serial|   5|    100000|     100|0.76608364|  PASSED
          sts_serial|   5|    100000|     100|0.95780908|  PASSED
          sts_serial|   6|    100000|     100|0.85591258|  PASSED
          sts_serial|   6|    100000|     100|0.92780508|  PASSED
          sts_serial|   7|    100000|     100|0.07612587|  PASSED
          sts_serial|   7|    100000|     100|0.04455050|  PASSED
          sts_serial|   8|    100000|     100|0.04477835|  PASSED
          sts_serial|   8|    100000|     100|0.57401068|  PASSED
          sts_serial|   9|    100000|     100|0.45451885|  PASSED
          sts_serial|   9|    100000|     100|0.95843894|  PASSED
          sts_serial|  10|    100000|     100|0.98011089|  PASSED
          sts_serial|  10|    100000|     100|0.90545433|  PASSED
          sts_serial|  11|    100000|     100|0.90248602|  PASSED
          sts_serial|  11|    100000|     100|0.82564692|  PASSED
          sts_serial|  12|    100000|     100|0.83679672|  PASSED
          sts_serial|  12|    100000|     100|0.99276836|  PASSED
          sts_serial|  13|    100000|     100|0.92541777|  PASSED
          sts_serial|  13|    100000|     100|0.61251288|  PASSED
          sts_serial|  14|    100000|     100|0.54998236|  PASSED
          sts_serial|  14|    100000|     100|0.25510650|  PASSED
          sts_serial|  15|    100000|     100|0.36926680|  PASSED
          sts_serial|  15|    100000|     100|0.48367702|  PASSED
          sts_serial|  16|    100000|     100|0.88802047|  PASSED
          sts_serial|  16|    100000|     100|0.65287953|  PASSED
         rgb_bitdist|   1|    100000|     100|0.82161472|  PASSED
         rgb_bitdist|   2|    100000|     100|0.40433518|  PASSED
         rgb_bitdist|   3|    100000|     100|0.89996191|  PASSED
         rgb_bitdist|   4|    100000|     100|0.66936247|  PASSED
         rgb_bitdist|   5|    100000|     100|0.09835458|  PASSED
         rgb_bitdist|   6|    100000|     100|0.23715415|  PASSED
         rgb_bitdist|   7|    100000|     100|0.00761607|  PASSED
         rgb_bitdist|   8|    100000|     100|0.88516483|  PASSED
         rgb_bitdist|   9|    100000|     100|0.76362355|  PASSED
         rgb_bitdist|  10|    100000|     100|0.42466435|  PASSED
         rgb_bitdist|  11|    100000|     100|0.99945611|   WEAK
         rgb_bitdist|  12|    100000|     100|0.99198516|  PASSED
rgb_minimum_distance|   2|     10000|    1000|0.98403892|  PASSED
rgb_minimum_distance|   3|     10000|    1000|0.27706149|  PASSED
rgb_minimum_distance|   4|     10000|    1000|0.28936467|  PASSED
rgb_minimum_distance|   5|     10000|    1000|0.11991040|  PASSED
    rgb_permutations|   2|    100000|     100|0.94187728|  PASSED
    rgb_permutations|   3|    100000|     100|0.72911009|  PASSED
    rgb_permutations|   4|    100000|     100|0.53541174|  PASSED
    rgb_permutations|   5|    100000|     100|0.72548431|  PASSED
      rgb_lagged_sum|   0|   1000000|     100|0.84092883|  PASSED
      rgb_lagged_sum|   1|   1000000|     100|0.95657028|  PASSED
      rgb_lagged_sum|   2|   1000000|     100|0.36449229|  PASSED
      rgb_lagged_sum|   3|   1000000|     100|0.13261664|  PASSED
      rgb_lagged_sum|   4|   1000000|     100|0.74507814|  PASSED
      rgb_lagged_sum|   5|   1000000|     100|0.70024498|  PASSED
      rgb_lagged_sum|   6|   1000000|     100|0.44743601|  PASSED
      rgb_lagged_sum|   7|   1000000|     100|0.76395544|  PASSED
      rgb_lagged_sum|   8|   1000000|     100|0.69111007|  PASSED
      rgb_lagged_sum|   9|   1000000|     100|0.26585560|  PASSED
      rgb_lagged_sum|  10|   1000000|     100|0.05891180|  PASSED
      rgb_lagged_sum|  11|   1000000|     100|0.29696531|  PASSED
      rgb_lagged_sum|  12|   1000000|     100|0.92960086|  PASSED
      rgb_lagged_sum|  13|   1000000|     100|0.98070688|  PASSED
      rgb_lagged_sum|  14|   1000000|     100|0.97492933|  PASSED
      rgb_lagged_sum|  15|   1000000|     100|0.80286922|  PASSED
      rgb_lagged_sum|  16|   1000000|     100|0.78809145|  PASSED
      rgb_lagged_sum|  17|   1000000|     100|0.54320014|  PASSED
      rgb_lagged_sum|  18|   1000000|     100|0.68304519|  PASSED
      rgb_lagged_sum|  19|   1000000|     100|0.38713781|  PASSED
      rgb_lagged_sum|  20|   1000000|     100|0.74346192|  PASSED
      rgb_lagged_sum|  21|   1000000|     100|0.95325771|  PASSED
      rgb_lagged_sum|  22|   1000000|     100|0.49630657|  PASSED
      rgb_lagged_sum|  23|   1000000|     100|0.45007730|  PASSED
      rgb_lagged_sum|  24|   1000000|     100|0.20258980|  PASSED
      rgb_lagged_sum|  25|   1000000|     100|0.95329245|  PASSED
      rgb_lagged_sum|  26|   1000000|     100|0.28014991|  PASSED
      rgb_lagged_sum|  27|   1000000|     100|0.39147833|  PASSED
      rgb_lagged_sum|  28|   1000000|     100|0.46353444|  PASSED
      rgb_lagged_sum|  29|   1000000|     100|0.83130355|  PASSED
      rgb_lagged_sum|  30|   1000000|     100|0.16565660|  PASSED
      rgb_lagged_sum|  31|   1000000|     100|0.61030257|  PASSED
      rgb_lagged_sum|  32|   1000000|     100|0.91739648|  PASSED
     rgb_kstest_test|   0|     10000|    1000|0.77890867|  PASSED
     dab_bytedistrib|   0|  51200000|       1|0.90627566|  PASSED
             dab_dct| 256|     50000|       1|0.86174430|  PASSED
Preparing to run test 207.  ntuple = 0
        dab_filltree|  32|  15000000|       1|0.04627158|  PASSED
        dab_filltree|  32|  15000000|       1|0.55980521|  PASSED
Preparing to run test 208.  ntuple = 0
       dab_filltree2|   0|   5000000|       1|0.61696902|  PASSED
       dab_filltree2|   1|   5000000|       1|0.46949689|  PASSED
Preparing to run test 209.  ntuple = 0
        dab_monobit2|  12|  65000000|       1|0.30692569|  PASSED

*/

#endif
