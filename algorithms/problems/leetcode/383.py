class Solution:
    def canConstruct(self, ransomNote: str, magazine: str) -> bool:
        can_construct=True
        notes={}
        for note in ransomNote:
            if note not in notes:
                notes[note]=0
            notes[note]+=1
        for note in notes:
            if notes[note]>magazine.count(note):
                can_construct=False
                break
        return can_construct
