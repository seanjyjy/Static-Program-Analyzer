<program>
	<procedure name=n2wwf>
		<stmts>
			<while>
				<not>
					<eq>
						<var name=tired></var>
						<var name=true></var>
					</eq>
				</not>
				<stmts>
					<while>
						<not>
							<eq>
								<var name=spirit></var>
								<var name=broken></var>
							</eq>
						</not>
						<stmts>
							<while>
								<ge>
									<var name=willToLive></var>
									<var name=willToDie></var>
								</ge>
								<stmts>
									<read>
										<var name=continueWorking></var>
									</read>
								</stmts>
							</while>
						</stmts>
					</while>
				</stmts>
			</while>
			<print>
				<var name=writeCS3203Tests></var>
			</print>
		</stmts>
	</procedure>
</program>
