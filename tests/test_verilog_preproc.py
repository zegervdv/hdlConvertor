import unittest
from os import path

try:
    from tests.basic_tc import TEST_DIR
except ImportError:
    from basic_tc import TEST_DIR

from hdlConvertor import ParseException, HdlConvertor
from hdlConvertor.language import Language

SV = Language.SYSTEM_VERILOG_2012


def _test_run(test_file, golden_file):
    c = HdlConvertor()

    incdirs = ['.', '..', path.join('sv_pp', 'src')]
    test_result = c.verilog_pp(
        test_file, incdirs, SV)

    with open(golden_file) as myfile:
        test_golden = myfile.read()

    return test_result, test_golden


def _test_run_rel(test_file, golden_file):
    return _test_run(
            path.join(TEST_DIR, 'sv_pp', 'src', test_file),
            path.join(TEST_DIR, 'sv_pp', 'expected', golden_file)
    )


class VerilogPreprocTC(unittest.TestCase):

    def assertPPWorks(self, test_file):
        golden_file = test_file  # same name but will get a diffent folder

        result, ref = _test_run_rel(test_file, golden_file)
        self.assertEqual(result, ref)

    def test_2012_p641(self):
        self.assertPPWorks('2012_p641.txt')

    def test_2012_p642(self):
        self.assertPPWorks('2012_p642.txt')

    def test_2012_p642_2(self):
        self.assertPPWorks('2012_p642_2.txt')

    def test_2012_p643(self):
        self.assertPPWorks('2012_p643.txt')

    def test_2012_p643_2(self):
        self.assertPPWorks('2012_p643_2.txt')

    def test_2012_p643_3(self):
        self.assertPPWorks('2012_p643_3.txt')

    def test_2012_p644(self):
        self.assertPPWorks('2012_p644.txt')

    def assertPPError(self, file, err_msg):
        with self.assertRaises(ParseException) as context:
            f = path.join(TEST_DIR, 'sv_pp', 'src', file)
            c = HdlConvertor()
            c.verilog_pp(
                f,
                ['.', '..', path.join('sv_pp', 'src')],
                SV
            )
        self.assertEqual(err_msg, context.exception.__str__())

    def test_2012_p644_2(self):
        # [TODO] platform dependent path
        self.assertPPError(
            '2012_p644_2.txt',
            '/home/mydir/myfile was not found in include directories\n'
        )

    def test_2012_p641_il1(self):
        self.assertPPError(
            '2012_p641_il1.txt',
            'Macro D missing value for parameter y'
            ' (2 arguments expected but 1 provided).'
        )

    def test_2012_p641_il2(self):
        self.assertPPError(
            '2012_p641_il2.txt',
            'Macro D missing value for parameter x and for parameters'
            ' after (2 arguments expected but 0 provided).'
        )

    def test_2012_p641_il3(self):
        self.assertPPError(
            '2012_p641_il3.txt',
            'Macro D expected 2 arguments but 3 provided.'
        )

    def test_2012_p642_il1(self):
        self.assertPPError(
            '2012_p642_il1.txt',
            'Macro MACRO1 missing value for parameter c'
            ' (1 to 3 arguments expected but 2 provided).'
        )

    # `MACRO3 must have parentesis according to the SV specification.
    def test_2012_p642_il2(self):
        self.assertPPError(
            '2012_p642_il2.txt',
            'Macro MACRO3 requires braces and expects (0 to 3 arguments).'
        )

    # No check that string are not split
    @unittest.expectedFailure
    def test_2012_p642_il3(self):
        self.assertPPError(
            '2012_p642_il3.txt',
            'an error message'
        )

    def test_FILE_LINE(self):
        c = HdlConvertor()
        f = path.join(path.dirname(__file__), 'sv_pp', 'src', 'test_FILE_LINE.sv')
        incdirs = ['.', '..', path.join('sv_pp', 'src')]
        test_result = c.verilog_pp(f, incdirs, SV)
        expected_val = path.join(path.dirname(__file__),
                                 'sv_pp', 'src', 'test_FILE_LINE.sv'
                                 )
        test_golden = ("module tb();\n\ninitial\n\t$display("
                      "\"Internal error: null handle at %s, line %d.\",\n")
        test_golden += "\"" + expected_val + "\", 5);\n\n\nendmodule\n"
        self.assertEqual(test_result, test_golden)



if __name__ == "__main__":
    suite = unittest.TestSuite()
    # suite.addTest(VerilogPreprocTC('test_2012_p641'))
    suite.addTest(unittest.makeSuite(VerilogPreprocTC))

    runner = unittest.TextTestRunner(verbosity=3)
    runner.run(suite)
