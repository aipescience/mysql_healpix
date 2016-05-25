
#ifndef __MYSQL_BOX_H__
#define __MYSQL_BOX_H__


class SBox {
public:
	SPoint  sw;   //!< South-west value of box
	SPoint  ne;   //!< North-east value of box

	SBox() : sw(SPoint(0.0, 0.0)), ne(SPoint(0.0, 0.0)) {}

	SBox( SPoint nSw, SPoint nNe ) {
		sw = nSw;
		ne = nNe;
	}
};

#endif

