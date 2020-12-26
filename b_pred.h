unsigned int index_calc_bimodal(unsigned int addr,unsigned int m2);
unsigned int prediction_bimodal(unsigned int counter_val);
unsigned int update_bimodal(unsigned int counter_val, unsigned int taken_nottaken);
unsigned int is_misprediction_bimodal(unsigned int predict_bimodal, unsigned int taken_nottaken,unsigned int misprediction_count_bimodal);

unsigned int index_calc_gshare(unsigned int addr,unsigned int gbh,unsigned int m1,unsigned int n);
unsigned int prediction_gshare(unsigned int counter_val);
unsigned int update_gbh(unsigned int predict_gshare,unsigned int gbh,unsigned int n);
unsigned int update_gshare(unsigned int counter_val, unsigned int taken_nottaken);
unsigned int is_misprediction_gshare(unsigned int predict_gshare, unsigned int taken_nottaken,unsigned int misprediction_count_gshare);

unsigned int index_calc_hybrid(unsigned int addr,unsigned int k);
unsigned int prediction_hybrid(unsigned int counter_value);
unsigned int update_chooser(unsigned int counter_val,unsigned int taken_nottaken, unsigned int predict_hybrid, unsigned int predict_gshare);
