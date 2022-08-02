class Solution:
    def badSensor(self, sensor1: List[int], sensor2: List[int]) -> int:
        bad_sensor = -1
        for i in range(len(sensor1) - 1):
            if sensor1[i] != sensor2[i]:
                if sensor1[i:-1] == sensor2[i+1:]:
                    bad_sensor = 1
                elif sensor1[i+1:] == sensor2[i:-1]:
                    bad_sensor = 2
                break
        return bad_sensor
