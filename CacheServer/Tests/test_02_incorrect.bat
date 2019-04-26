echo  | nc localhost 9876
echo INVALID REQUEST | nc localhost 9876
echo GET | nc localhost 9876
echo GET ERRORKEY=1 | nc localhost 9876
echo GET VALUE=5 | nc localhost 9876
echo GET KEY error | nc localhost 9876
echo GET KEY=456 VALUE=6 TTL=4 | nc localhost 9876
echo PUT | nc localhost 9876
echo PUT ERRORKEY=1 | nc localhost 9876
echo PUT KEY=456 | nc localhost 9876
echo PUT KEY=456 TTL=4 VALUE=6 | nc localhost 9876
echo PUT KEY=456 VALUE=6 TTL=4 ADDITIONALPARAM=x | nc localhost 9876
echo PUT KEY=456 VALUE=6 TTL=INVALID_TTL | nc localhost 9876
