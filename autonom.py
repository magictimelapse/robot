#!/usr/bin/env python

import PicoBorgRev
import XLoBorg




if __name__ == "__main__":
    PBR = PicoBorgRev.PicoBorgRev()
    PBR.Init()
    PBR.SetCommsFailsafe(False)
    PBR.ResetEpo()
    XLoBorg.printFunction = XLoBorg.NoPrint
    XLoBorg.tempOffset =25
    XLoBorg.Init()
    x,y,z = XLoBorg.ReadAccelerometer()
