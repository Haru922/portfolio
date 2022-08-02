class Solution:
    def slowestKey(self, releaseTimes: List[int], keysPressed: str) -> str:
        max_duration=prev=0
        for i in range(len(releaseTimes)):
            duration=releaseTimes[i]-prev
            prev=releaseTimes[i]
            releaseTimes[i]=duration
            max_duration=max(max_duration,duration)
        key=''
        for i in range(len(releaseTimes)):
            if releaseTimes[i]==max_duration:
                key=max(key,keysPressed[i])
        return key
