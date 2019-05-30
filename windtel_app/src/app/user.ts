export class User {

    constructor(
        public rid: number,
        public rname: string,
        public rlast_name: string,
        public remail: string,
        public rvocation: string,
        public rdepartment: string,
        public rinstitution: string,
        public rcity: string,
        public rpassword: string,
        public rpermissions: string
    ) { }
}
