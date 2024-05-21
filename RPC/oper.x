struct in1 {
    int y<100>;
};

struct in2 {
    double a;
    int y<100>;
};

struct out1 {
    int maxmin[2];
};

struct out2 {
    double pr<100>;
};

program ADD_PROG {
                version ADD_VERS {
                    double AVG(in1) = 1;
                    out1 MAXMIN(in1) = 2;
                    out2 PRODay(in2) = 3;
                } = 1;
} = 0x23451111;