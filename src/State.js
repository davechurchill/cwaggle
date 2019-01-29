// Reinforcement Learning GridWorld Environment

let HASH_BITS = new Array(16).fill(0);

class RobotState {

    constructor(sensorReadings) {
        this.puckLeft       = sensorReadings.leftRedPuck.count;
        this.puckRight      = sensorReadings.rightRedPuck.count;
        this.obsLeft        = sensorReadings.leftObstacle.count;
        this.obsRight       = sensorReadings.rightObstacle.count;
        this.scalarLeft     = sensorReadings.leftProbe.nestValue;
        this.scalarRight    = sensorReadings.rightProbe.nestValue;
        this.scalarMid      = sensorReadings.centreProbe.nestValue;

        this.hash = this.computeHash();
    }

    // returns the 16-bit int hash value of a state
    computeHash() {

        let sum = 0;
        
        sum += (this.puckLeft     == 0) ? 0  : (1 <<  0);
        sum += (this.puckRight    == 0) ? 0  : (1 <<  1);
        sum += (this.obsLeft      == 0) ? 0  : (1 <<  2);
        sum += (this.obsRight     == 0) ? 0  : (1 <<  3);
        sum += ((this.scalarLeft  * 15) | 0) * (1 <<  4);
        sum += ((this.scalarRight * 15) | 0) * (1 <<  8);
        sum += ((this.scalarMid   * 15) | 0) * (1 << 12);

        if (sum >= (1 << 16)) { console.log("HASH ERROR", sum); }
        return sum;
    }
}