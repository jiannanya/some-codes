<?php

/**

 * DES encryption algorithm

 * date: 2020/4/29

 */

namespace app\components;

class DesComponents{

    public $desKey;

    public function __construct(){
        $this->desKey = '12345678';
    }
    //DES 加密
    public function  des($encrypt) {
        $passcrypt = openssl_encrypt($encrypt, 'DES-ECB', $this->desKey, OPENSSL_RAW_DATA);
        return $passcrypt;
    }
    /**

     * 将二进制数据转换成十六进制

     */
    public function asc2hex($temp) {

        return bin2hex ( $temp );
    }
    /**
     * 十六进制转换成二进制
     *
     * @param string
     * @return string
     */
    public function hex2asc($temp) {
        $len = strlen ( $temp );
        $data = '';
        for($i = 0; $i < $len; $i += 2)
            $data .= chr ( hexdec ( substr ( $temp, $i, 2 ) ) );
        return $data;
    }
    //DES解密
    public function un_des($decrypt) {
        $cipher = openssl_decrypt(($decrypt), 'DES-ECB', $this->desKey, OPENSSL_RAW_DATA);
        $cipher = trim($cipher);
        return $cipher;
    }
}
