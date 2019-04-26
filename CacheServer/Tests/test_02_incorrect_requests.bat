echo  | nc localhost 9876 > output_02.txt
echo INVALID REQUEST | nc localhost 9876 >> output_02.txt
echo GET | nc localhost 9876 >> output_02.txt
echo GET ERRORKEY=1 | nc localhost 9876 >> output_02.txt
echo GET VALUE=5 | nc localhost 9876 >> output_02.txt
echo GET KEY error | nc localhost 9876 >> output_02.txt
echo GET KEY=456 VALUE=6 TTL=4 | nc localhost 9876 >> output_02.txt
echo PUT | nc localhost 9876 >> output_02.txt
echo PUT ERRORKEY=1 | nc localhost 9876 >> output_02.txt
echo PUT KEY=456 | nc localhost 9876 >> output_02.txt
echo PUT KEY=456 TTL=4 VALUE=6 | nc localhost 9876 >> output_02.txt
echo PUT KEY=456 VALUE=6 TTL=4 ADDITIONALPARAM=x | nc localhost 9876 >> output_02.txt
echo PUT KEY=456 VALUE=6 TTL=INVALID_TTL | nc localhost 9876 >> output_02.txt
