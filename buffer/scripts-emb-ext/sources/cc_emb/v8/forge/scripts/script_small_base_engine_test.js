database.PABTotal_ = 3;
database.total_bcl_ = 4;
database.excitersTotal_ = 3;
database.DBTotal_ = 2;
database.transmitterID___ = 4;
database.terminal_amps_per_block___=2;
database.sizeBlockPreampParams_ = 2;
database.sizeBlockTerminalAmpParams_ = 3;
database.sizeBlockBCNParams_ = 3;
database.sizeBlockDBParams_ = 2;	
database.sizeBlockModParams_ = 2;

/*
log("JS log: "+database.temp)
log("idx_oned_etv_ = " + database.idx_oned_etv_)
log("uchar = " + database.var_char)*/

database.ibRadioMode_=5
if (database.ibRadioMode_ === 5) {
	database.ibRadioMode_=6
}
//log(database.ibRadioMode_)

var time = 0
function Process() {
	log("time = " + time)
	if (time === 0) {
		log("switch on");
		database.idx_oned_etv_ = 1	;
	}
	if (time === 1) {
		database.ibRadioMode_=2;
		log("unlock");
	}
	time = time + 1;
}

log(database.ibRadioMode_);
//log("gel")